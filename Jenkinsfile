pipeline {
    agent none

    stages {

        stage('Build') {
            parallel {
                stage('Windows') {
                    agent { label 'windows' }
                    
                    steps {
                        bat 'gradlew.bat shadow'
                    }
                }

                stage ('Linux') {
                    agent { 
                        docker { image 'openjdk:8-alpine' }
                    }

                    steps {
                        sh './gradlew'
                    }
                }
            }
        }
    }
}
        