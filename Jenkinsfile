pipeline {
    agent none

    options { checkoutToSubdirectory('cse-core4j') }

    stages {

        stage('Build') {
            parallel {
                stage('Windows') {
                    agent { label 'windows' }
                    
                    steps {
                        bat 'gradlew.bat clean build'
                    }
                }

                stage ('Linux') {
                    agent { 
                        dockerfile {
                            filename 'Dockerfile.build'
                            dir 'cse-core4j/.dockerfiles'
                            label 'linux && docker'
                        }
                    }

                    steps {
                        sh './gradlew clean build'
                    }
                }
            }
        }
    }
}
        